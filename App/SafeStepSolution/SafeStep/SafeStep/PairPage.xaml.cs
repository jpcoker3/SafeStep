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
	public partial class PairPage : ContentPage
	{
		public PairPage ()
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


}
/*
BluetoothAdapter adapter = BluetoothAdapter.DefaultAdapter;
if (adapter == null)
    throw new Exception("No Bluetooth adapter found.");

if (!adapter.IsEnabled)
    throw new Exception("Bluetooth adapter is not enabled.");

BluetoothDevice device = (from bd in adapter.BondedDevices 
                      where bd.Name == "NameOfTheDevice" select bd).FirstOrDefault();

if(device == null)
   throw new Exception("Named device not found.");


_socket = device.CreateRfcommSocketToServiceRecord(UUID.FromString("00001101-0000-1000-8000-00805f9b34fb"));

await _socket.ConnectAsync();


// Read data from the device
await _socket.InputStream.ReadAsync(buffer, 0, buffer.Length);

// Write data to the device
await _socket.OutputStream.WriteAsync(buffer, 0, buffer.Length);
*/