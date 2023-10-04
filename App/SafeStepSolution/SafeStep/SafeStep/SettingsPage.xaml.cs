using SafeStep.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace SafeStep
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SettingsPage : ContentPage
    {
        public SettingsPage()
        {

            InitializeComponent();

            var displayInfo = DeviceDisplay.MainDisplayInfo;
            var screenWidth = displayInfo.Width;
            var screenHeight = displayInfo.Height;
            var NavBarheight = screenHeight / 25;
            var buttonWidth = screenWidth / 13;

            Status.WidthRequest = buttonWidth;
            Locate.WidthRequest = buttonWidth;
            Pair.WidthRequest = buttonWidth;
            Settings.WidthRequest = buttonWidth;

            NavBar.WidthRequest = screenWidth;
            NavBar.HeightRequest = NavBarheight;

        }

        // Navigate to Credits page.
        async void NavigateToCredits(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new CreditsPage()));
        }


        // Navigate to settings page.
        async void NavigateToSettings(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new SettingsPage()));
        }

        // Navigate to status page.
        async void NavigateToStatus(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new StatusPage()));
        }

        // Navigate to Locate page.
        async void NavigateToLocate(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new LocatePage()));
        }

        // Navigate to settings page.
        async void NavigateToPair(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new PairPage()));
        }
        async void ScheduleNotificationButtonClicked(object sender, EventArgs e)
        {
            // Call the ScheduleLocalNotification method to schedule the notification.
            ScheduleLocalNotification();
        }
        private void ScheduleLocalNotification()
        {
            // Get an instance of the notification service through DependencyService
            var notificationService = DependencyService.Get<INotificationService>();

            // Define notification details
            string title = "Notification Title";
            string message = "This is the notification message.";
            int notificationId = 1; // A unique ID for the notification
            DateTime notifyTime = DateTime.Now.AddSeconds(10); // Set the time when the notification should appear

            // Call the ShowNotification method to schedule the notification
            notificationService?.ShowNotification(title, message, notificationId, notifyTime);
        }
    }

}