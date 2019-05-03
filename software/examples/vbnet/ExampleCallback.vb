Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Color Bricklet 2.0

    ' Callback subroutine for color callback
    Sub ColorCB(ByVal sender As BrickletColorV2, ByVal r As Integer, ByVal g As Integer, _
                ByVal b As Integer, ByVal c As Integer)
        Console.WriteLine("Color [R]: " + r.ToString())
        Console.WriteLine("Color [G]: " + g.ToString())
        Console.WriteLine("Color [B]: " + b.ToString())
        Console.WriteLine("Color [C]: " + c.ToString())
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim c As New BrickletColorV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register color callback to subroutine ColorCB
        AddHandler c.ColorCallback, AddressOf ColorCB

        ' Set period for color callback to 0.1s (100ms)
        c.SetColorCallbackConfiguration(100, False)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
