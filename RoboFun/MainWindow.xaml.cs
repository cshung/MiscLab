namespace RoboFun
{
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Media;
    using System.Windows.Shapes;
    using System.Windows.Threading;

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private double firstAngle = +Math.PI / 2;
        private double secondAngle = +Math.PI / 2;
        private Rectangle firstArm = new Rectangle();
        private Rectangle secondArm = new Rectangle();
        private List<Point> recordingPoints;
        private List<Point> recordedPoints;
        private int replayIndex;
        private DispatcherTimer timer;

        public MainWindow()
        {
            InitializeComponent();

            Line xAxis = new Line(); xAxis.X1 = 0; xAxis.Y1 = 200; xAxis.X2 = 400; xAxis.Y2 = 200; xAxis.Stroke = Brushes.Black;
            Line yAxis = new Line(); yAxis.X1 = 200; yAxis.Y1 = 0; yAxis.X2 = 200; yAxis.Y2 = 400; yAxis.Stroke = Brushes.Black;

            Ellipse workspace = new Ellipse();
            workspace.Width = 360;
            workspace.Height = 360;
            workspace.Fill = Brushes.AntiqueWhite;
            Canvas.SetLeft(workspace, 20);
            Canvas.SetTop(workspace, 20);

            Canvas.SetLeft(firstArm, 0);
            Canvas.SetTop(firstArm, 0);
            firstArm.Width = 100;
            firstArm.Height = 20;
            firstArm.Fill = Brushes.Red;
            Canvas.SetLeft(secondArm, 0);
            Canvas.SetTop(secondArm, 0);
            secondArm.Width = 100;
            secondArm.Height = 20;
            secondArm.Fill = Brushes.Blue;

            this.ComputeTransforms();

            this.Canvas.Children.Add(workspace);
            this.Canvas.Children.Add(xAxis);
            this.Canvas.Children.Add(yAxis);
            this.Canvas.Children.Add(firstArm);
            this.Canvas.Children.Add(secondArm);
        }

        private void ComputeTransforms()
        {
            TransformGroup firstArmTransforms = new TransformGroup();
            firstArmTransforms.Children.Add(new RotateTransform(-firstAngle * 180 / Math.PI, 10, 10)); // convention of center is offset from left top corner, WPF rotate transform is clockwise and in angles.
            firstArmTransforms.Children.Add(new TranslateTransform(190, 190)); // align (10, 10) with (200, 200), the center of the canvas)

            double secondArmX = 190 + Math.Cos(firstAngle) * 80; // WPF coordinate has Y go upside down
            double secondArmY = 190 - Math.Sin(firstAngle) * 80;

            TransformGroup secondArmTransforms = new TransformGroup();
            secondArmTransforms.Children.Add(new RotateTransform(-(firstAngle + secondAngle) * 180 / Math.PI, 10, 10)); // convention of center is offset from left top corner, WPF rotate transform is clockwise and in angles.
            secondArmTransforms.Children.Add(new TranslateTransform(secondArmX, secondArmY));

            firstArm.RenderTransform = firstArmTransforms;
            secondArm.RenderTransform = secondArmTransforms;
        }

        private void OnRunButtonClicked(object sender, RoutedEventArgs e)
        {
            if (this.recordedPoints != null && this.recordedPoints.Count > 0)
            {
                replayIndex = 0;
                this.timer = new DispatcherTimer();
                this.timer.Interval = TimeSpan.FromMilliseconds(30);
                this.timer.Start();
                this.timer.Tick += OnTimerTick;

                List<UIElement> lines = new List<UIElement>();
                foreach (UIElement child in this.Canvas.Children)
                {
                    if (child is Line)
                    {
                        lines.Add(child);
                    }
                }
                foreach (var line in lines)
                {
                    this.Canvas.Children.Remove(line);
                }
            }
        }

        void OnTimerTick(object sender, EventArgs e)
        {
            if (replayIndex < recordedPoints.Count)
            {
                Line traj = new Line();
                traj.X1 = 200 + (80 * Math.Cos(this.firstAngle) + 80 * Math.Cos(this.secondAngle + this.firstAngle));
                traj.Y1 = 200 - (80 * Math.Sin(this.firstAngle) + 80 * Math.Sin(this.secondAngle + this.firstAngle));

                double screenX = this.recordedPoints[replayIndex].X;
                double screenY = this.recordedPoints[replayIndex].Y;

                double worldX = screenX - 200;
                double worldY = 200 - screenY;

                this.secondAngle = Math.Abs(Math.Acos((worldX * worldX + worldY * worldY - 80 * 80 - 80 * 80) / (2 * 80 * 80)));
                this.firstAngle = Math.Atan2(worldY, worldX) - Math.Atan2(80 * Math.Sin(this.secondAngle), 80 + 80 * Math.Cos(this.secondAngle));
                this.ComputeTransforms();

                traj.X2 = 200 + (80 * Math.Cos(this.firstAngle) + 80 * Math.Cos(this.secondAngle + this.firstAngle));
                traj.Y2 = 200 - (80 * Math.Sin(this.firstAngle) + 80 * Math.Sin(this.secondAngle + this.firstAngle));
                traj.Stroke = Brushes.Black;
                if (replayIndex != 0)
                {
                    this.Canvas.Children.Add(traj);
                }
                this.ComputeTransforms();
                replayIndex++;
            }
            else
            {
                this.timer.Stop();
            }
        }

        private void OnCanvasMouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            this.recordingPoints = new List<Point>();
            this.recordedPoints = null;
        }

        private void OnCanvasMouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {           
            this.recordedPoints = this.recordingPoints;
            this.recordingPoints = null;
        }

        private void OnCanvasMouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (this.recordingPoints != null)
            {
                this.recordingPoints.Add(e.GetPosition(this.Canvas));
            }
        }
    }
}
