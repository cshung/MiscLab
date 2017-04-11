using Android.App;
using Android.Widget;
using Android.OS;
using Android.Media;
using System;

namespace Experiment
{
	public class Listener : Java.Lang.Object, MediaPlayer.IOnCompletionListener
	{
		private Action action;

		public Listener(Action action)
		{
			this.action = action;
		}
		
		public void OnCompletion(MediaPlayer unused)
		{
			this.action();
		}
	}

	[Activity(Label = "Experiment", MainLauncher = true, Icon = "@mipmap/icon")]
	public class MainActivity : Activity
	{
		private VideoView foreView;
		private VideoView backView;

		protected override void OnCreate(Bundle savedInstanceState)
		{
			base.OnCreate(savedInstanceState);

			// Set our view from the "main" layout resource
			SetContentView(Resource.Layout.Main);

			this.foreView = this.FindViewById<VideoView>(Resource.Id.foreView);
			this.backView = this.FindViewById<VideoView>(Resource.Id.backView);

			this.foreView.SetVideoURI(Android.Net.Uri.Parse("..."));
			this.foreView.Start();
			this.foreView.SetOnCompletionListener(new Listener(this.OnForeViewCompleted1));

			this.backView.SetVideoURI(Android.Net.Uri.Parse("..."));
			this.backView.SetOnCompletionListener(new Listener(this.OnBackViewCompleted1));
		}

		private void OnForeViewCompleted1()
		{
			this.backView.Start();
			this.foreView.SetVideoURI(Android.Net.Uri.Parse("..."));
			this.backView.BringToFront();
		}

		private void OnBackViewCompleted1()
		{
			this.foreView.SetOnCompletionListener(null);
			this.foreView.Start();
			this.foreView.BringToFront();

		}
	}
}
