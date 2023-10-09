namespace SafeStepMAUI;

public partial class SettingsPage : ContentPage
{
	public SettingsPage()
	{
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
}