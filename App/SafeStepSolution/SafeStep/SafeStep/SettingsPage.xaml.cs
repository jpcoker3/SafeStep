using Plugin.LocalNotification;
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
            await Navigation.PushModalAsync(new NavigationPage(new CreditsPage()), false);
        }


        // Navigate to settings page.
        async void NavigateToSettings(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new SettingsPage()), false);
        }

        // Navigate to status page.
        async void NavigateToStatus(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new StatusPage()), false);
        }

        // Navigate to Locate page.
        async void NavigateToLocate(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new LocatePage()), false);
        }

        // Navigate to settings page.
        async void NavigateToPair(object sender, EventArgs e)
        {
            NavigationPage.SetHasNavigationBar(this, false);
            await Navigation.PushModalAsync(new NavigationPage(new PairPage()), false);
        }
        void SendNotification(System.Object sender, System.EventArgs e)
        {
            var notification = new NotificationRequest
            {
                Title = "Notification",
                Description = "Test Description",
                ReturningData = "Dummy Data",
                BadgeNumber = 1,
                NotificationId = 1337,
                Schedule = new NotificationRequestSchedule
                {
                    NotifyTime = DateTime.Now.AddSeconds(5)
                }
            };

            LocalNotificationCenter.Current.Show(notification);
        }

    }
}