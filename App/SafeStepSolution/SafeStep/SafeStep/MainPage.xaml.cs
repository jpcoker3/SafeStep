using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace SafeStep
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();

        }

        int count = 0;
        void Button_Clicked(object sender, System.EventArgs e)
        {
            count++;
            ((Button)sender).Text = $"You clicked {count} times.";
        }

        // Navigate to settings page.
        async void NavigateToSettings(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new NavigationPage(new SettingsPage()));
        }

        // Navigate to status page.
        async void NavigateToStatus(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new NavigationPage(new StatusPage()));
        }

        // Navigate to Locate page.
        async void NavigateToLocate(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new NavigationPage(new LocatePage()));
        }

        // Navigate to settings page.
        async void NavigateToPair(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new NavigationPage(new PairPage()));
        }


    }

}

