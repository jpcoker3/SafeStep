using Microsoft.Maui.Devices;

namespace SafeStepMAUI;


public partial class StatusPage : ContentPage
{
	public StatusPage()
    {

  
    }

    // Navigate to settings page.
    async void NavigateToSettings(object sender, EventArgs e)
    {
        await Navigation.PushModalAsync(new SettingsPage());
    }

    async void NavigateToStatus(object sender, EventArgs e)
    {
        await Navigation.PushModalAsync(new StatusPage());
    }

    async void NavigateToLocate(object sender, EventArgs e)
    {
        await Navigation.PushModalAsync(new LocatePage());
    }

    async void NavigateToPair(object sender, EventArgs e)
    {
        await Navigation.PushModalAsync(new PairPage());
    }

}
