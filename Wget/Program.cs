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
                    var request = new HttpRequestMessage(HttpMethod.Post, uri);
                    var sendTask = client.SendAsync(request, HttpCompletionOption.ResponseHeadersRead);
                    var response = sendTask.Result.EnsureSuccessStatusCode();
                    string rawFileNameFromResponse = GetFileName(response);
                    string decodedFileName = WebUtility.UrlDecode(rawFileNameFromResponse);

                    var httpStream = await response.Content.ReadAsStreamAsync();

                    using (var fileStream = File.Create(decodedFileName))
                    {
                        using (var reader = new StreamReader(httpStream))
                        {
                            httpStream.CopyTo(fileStream);
                            fileStream.Flush();
                        }
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