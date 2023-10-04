using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace SafeStep.Utilities
{
    public static class Utilities
    {
        public static async Task SlideAsync(Page pageToPush, Page pageToPop)
        {
            // Slide animation from right to left
            await pageToPush.TranslateTo(0, 0, 0);
            await pageToPush.TranslateTo(pageToPush.Width, 0, 500, Easing.Linear);
            await pageToPop.Navigation.PushAsync(pageToPush, false);
            await pageToPush.TranslateTo(0, 0, 250, Easing.Linear);
        }
    }
    public interface INotificationService
    {
        void ShowNotification(string title, string message, int notificationId, DateTime notifyTime);
    }
}
