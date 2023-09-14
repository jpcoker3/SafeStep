using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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