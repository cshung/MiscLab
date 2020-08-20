namespace Wget
{
    using System;
    using System.IO;
    using System.Net;
    using System.Net.Http;
    using System.Threading.Tasks;

    internal static class Program
    {
        private static void Main(string[] args)
        {
            MainAsync(args).Wait();
        }

        private static async Task MainAsync(string[] args)
        {
            if (args.Length == 1)
            {
                string address = args[0];
                await DownloadFileAsync(new Uri(address));
            }
            else
            {
                Console.Error.WriteLine("Usage: wget address");
            }
        }

        public static string GetFileName(HttpResponseMessage response)
        {
            if (response.Content.Headers.ContentDisposition == null)
            {
                return Guid.NewGuid() + ".dat";
            }
            else
            {
                return response.Content.Headers.ContentDisposition.FileName;
            }
        }

        private static async Task DownloadFileAsync(Uri uri)
        {
            try
            {
                using (var client = new HttpClient())
                {
                    client.Timeout = TimeSpan.FromMinutes(5);
                    var request = new HttpRequestMessage(HttpMethod.Get, uri);
                    var sendTask = client.SendAsync(request, HttpCompletionOption.ResponseHeadersRead);
                    var response = sendTask.Result.EnsureSuccessStatusCode();
                    long? length = response.Content.Headers.ContentLength;
                    string rawFileNameFromResponse = GetFileName(response);
                    string decodedFileName = WebUtility.UrlDecode(rawFileNameFromResponse);
                    var httpStream = await response.Content.ReadAsStreamAsync();
                    long count = 0;
                    int lastProgress = 0;
                    using (var fileStream = File.Create(decodedFileName))
                    {
                        byte[] buffer = new byte[4 * 1024 * 1024];
                        while (true)
                        {
                            int bytesRead = await httpStream.ReadAsync(buffer, 0, 1024).ConfigureAwait(false);
                            if (length != null)
                            {
                                count += bytesRead;
                                int progress = (int)(count * 100 / length);
                                if (progress != lastProgress)
                                {
                                    Console.WriteLine(progress);
                                    lastProgress = progress;
                                }
                            }
                            if (bytesRead == 0) break;
                            await fileStream.WriteAsync(buffer, 0, bytesRead).ConfigureAwait(false);
                        }
                        fileStream.Flush();
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error, try again!");
                Console.WriteLine(ex);
            }
        }

    }
}