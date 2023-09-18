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
	public partial class StatusPage : ContentPage
	{
		public StatusPage ()
		{
			InitializeComponent ();

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
    }
}