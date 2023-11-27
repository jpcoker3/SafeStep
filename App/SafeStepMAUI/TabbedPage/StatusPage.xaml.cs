
using Microsoft.Maui.Controls;
using System;
using System.IO;
using System.Text;
using System.Threading;
using System.Diagnostics;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Microsoft.Maui.Controls;
namespace TabbedPageSample;
public partial class StatusPage : ContentPage
{
    private Random random;
    private System.Threading.Timer updateTimer;

    //private readonly IBluetoothLE _bluetooth;
    //private readonly IAdapter _adapter;
    public StatusPage()
	{
		InitializeComponent();

        random = new Random();
        updateTimer = new System.Threading.Timer(OnTimerCallback, null, TimeSpan.Zero, TimeSpan.FromSeconds(10));


        //_bluetooth = CrossBluetoothLE.Current;
        //_adapter = CrossBluetoothLE.Current.Adapter;

    }


    private void OnTimerCallback(object state)
    {
        MainThread.BeginInvokeOnMainThread(() =>
        {
            // Update values with random readings within specified ranges
            setDecibel(random.Next(60, 110)); // Range: 60 - 120 dB
            setBatteryLevel(random.NextDouble()); // Range: 0.0 - 1.0
            setTemperature(random.Next(55, 86)); // Range: 55 - 85°F
        });
    }


    protected override void OnAppearing()
    {
        base.OnAppearing();

        setDecibel(75);
        setBatteryLevel(0.50);
        setTemperature(85);

        // Initiate Bluetooth connection
        //ConnectBluetooth();



    }
    public void setDecibel(float decibelReading) // any int
    {

        //Decibel reading between 60 -120 dB. 
        var decibelProgress = Math.Round((float)decibelReading, 2);

        decibelProgressBar.Progress = (double)decibelProgress / 100; // 120 is max decibels. 
        decibelLabel.Text = "Decibel Level: " + decibelReading + " db"; // describe current decibels

        if (decibelProgress >= 85)
        {
            decibelProgressBar.ProgressColor = Colors.Red;
        }
        else if (decibelProgress > 25)
        {
            decibelProgressBar.ProgressColor = Colors.Green;
        }
        else
        {
            decibelProgressBar.ProgressColor = Colors.Blue;
        }

    }

    public void setTemperature(float tempReading) // any int
    {

        //so (number -55)/30 should give us a range between 0-1 for the range 0 - 120.
        var tempProgress = Math.Round(((float)tempReading / 100), 2);
        temperatureProgressBar.Progress = (double)tempProgress;
        temperatureLabel.Text = "Heat Index: " + tempReading + "°F";

        if (tempProgress > 0.80)
        {
            temperatureProgressBar.ProgressColor = Colors.Red;
        }
        else if (tempProgress > 0.60)
        {
            temperatureProgressBar.ProgressColor = Colors.Green;
        }
        else
        {
            temperatureProgressBar.ProgressColor = Colors.Red;
        }

    }

    public void setBatteryLevel(double batteryReading) // 0-1
    {

        batteryProgressBar.Progress = batteryReading; // between 0 and 1
        batteryProgressLabel.Text = "Battery Level: " + Math.Round(batteryReading * 100,0) + "%"; // copy the data from progress bar
        //control battery color
        if (batteryReading > 0.75)
        {
            batteryProgressBar.ProgressColor = Colors.Green;
        }
        else if (batteryReading > 0.25)
        {
            batteryProgressBar.ProgressColor = Colors.Orange;
        }
        else
        {
            batteryProgressBar.ProgressColor = Colors.Red;
        }
    }

    /*
    public async Task StartScanning()
    {
        _adapter.DeviceDiscovered += (s, a) =>
        {
            Debug.WriteLine($"Device found: {a.Device.Name}");
            // Handle discovered device here
        };

        await _adapter.StartScanningForDevicesAsync();
    }
    public async Task ConnectToDevice(Guid deviceId)
    {
        var device = await _adapter.ConnectToKnownDeviceAsync(deviceId);
        // Handle connected device here
    }

    public async Task ReadCharacteristic(Guid serviceId, Guid characteristicId)
    {
        var connectedDevices = _adapter.ConnectedDevices;
        if (connectedDevices != null && connectedDevices.Any())
        {
            var device = connectedDevices.FirstOrDefault(); // Assuming you have a connected device

            var service = await device.GetServiceAsync(serviceId);
            if (service != null)
            {
                var characteristic = await service.GetCharacteristicAsync(characteristicId);
                if (characteristic != null)
                {
                    var bytes = await characteristic.ReadAsync();
                    // Handle read data here
                }
            }
        }
    }
    */

    /*
    private void ReceiveData()
    {
        var stream = client.GetStream();
        byte[] receive = new byte[1024];

        while (true)
        {
            Array.Clear(receive, 0, receive.Length);
            var readMessage = "";
            do
            {

                int bytesRead = stream.Read(receive, 0, receive.Length);
                readMessage += Encoding.ASCII.GetString(receive, 0, bytesRead);
            }
            while (stream.DataAvailable);


            // at this point, the readMessage string will contain the entire message. 

            // Split the received message by commas to extract values
            string[] values = readMessage.Split(',');

            string humidity = "", decibel = "", latitude = "", longitude = "";
            bool fallen = false;

            foreach (string value in values)
            {
                if (value.StartsWith("hum:"))
                {
                    humidity = value.Substring(4);
                    System.Diagnostics.Debug.WriteLine($"Humidity: {humidity}");
                }
                else if (value.StartsWith("dec:"))
                {
                    decibel = value.Substring(4);
                    System.Diagnostics.Debug.WriteLine($"Decibel: {decibel}");
                }
                else if (value.StartsWith("coords:"))
                {
                    string[] coords = value.Substring(7).Split('x');
                    if (coords.Length == 2)
                    {
                        latitude = coords[0];
                        longitude = coords[1];
                        System.Diagnostics.Debug.WriteLine($"Latitude: {latitude}, Longitude: {longitude}");
                    }
                }
                else if (value.StartsWith("fall:true"))
                {
                    fallen = true;
                    System.Diagnostics.Debug.WriteLine("Fallen: true");
                }
                else if (value.StartsWith("fall:false"))
                {
                    fallen = false;
                    System.Diagnostics.Debug.WriteLine("Fallen: false");
                }
            }

        }

    }
    */

}


