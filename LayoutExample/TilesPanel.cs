using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace LayoutExample
{
    public sealed class TilesPanel : Panel
    {
        int margin = 20;
        private Nullable<Size> arrangeSize;

        protected override Size MeasureOverride(Size availableSize)
        {
            Size s = base.MeasureOverride(availableSize);
            if (arrangeSize.HasValue)
            {
                double totalWidth = arrangeSize.Value.Width;
                double x = margin;
                double y = margin;
                bool first = true;
                double maxHeight = 0;

                foreach (FrameworkElement element in this.Children)
                {
                    element.Measure(availableSize);
                    double begin = x;
                    x += element.DesiredSize.Width;
                    x += margin;
                    if (x < totalWidth || first)
                    {
                        maxHeight = Math.Max(maxHeight, element.DesiredSize.Height);
                    }
                    first = false;
                    if (x >= totalWidth)
                    {
                        first = true;
                        x = margin;
                        y = y + maxHeight + margin;
                    }
                }

                return new Size(0, y + maxHeight + margin);
            }
            else
            {
                return s;
            }
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            if (arrangeSize.HasValue)
            {
                arrangeSize = null;
                // Size and position the child elements
                double totalWidth = finalSize.Width;
                double x = margin;
                double y = margin;
                bool first = true;
                double maxHeight = 0;

                foreach (FrameworkElement element in this.Children)
                {
                    double begin = x;
                    x += element.DesiredSize.Width;
                    x += margin;
                    if (x < totalWidth || first)
                    {
                        element.Arrange(new Rect(begin, y, element.DesiredSize.Width, element.DesiredSize.Height));
                        maxHeight = Math.Max(maxHeight, element.DesiredSize.Height);
                    }
                    first = false;
                    if (x >= totalWidth)
                    {
                        first = true;
                        x = margin;
                        y = y + maxHeight + margin;
                    }
                }

                return new Size(finalSize.Width, Math.Max(finalSize.Height, y));
            }
            else
            {
                this.arrangeSize = finalSize;
                this.InvalidateMeasure();
                return finalSize;
            }
        }
    }
}
