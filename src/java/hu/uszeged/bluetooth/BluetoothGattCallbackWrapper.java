package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattCharacteristic;
import android.util.Log;

final class BluetoothGattCallbackWrapper extends BluetoothGattCallback {
    private static final String TAG = "BluetoothGattCallbackWrapper";

    private BluetoothDeviceWrapper mDevice;

    public BluetoothGattCallbackWrapper(BluetoothDeviceWrapper device) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mDevice = device;
    }

    public static BluetoothGattCallbackWrapper create(
            BluetoothDeviceWrapper device) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothGattCallbackWrapper(device);
    }

    @Override
    public void onCharacteristicChanged(
            BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "onCharacteristicChanged");
        Log.i(TAG, "###################>");
    }

    @Override
    public void onCharacteristicRead(
            BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "onCharacteristicRead");
        Log.i(TAG, "###################>");
    }

    @Override
    public void onCharacteristicWrite(
            BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "onCharacteristicWrite");
        Log.i(TAG, "###################>");
    }

    @Override
    public void onDescriptorRead(
            BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "onDescriptorRead");
        Log.i(TAG, "###################>");
    }

    @Override
    public void onDescriptorWrite(
            BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "onDescriptorWrite");
        Log.i(TAG, "###################>");
    }

    @Override
    public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "#### onConnectionStateChange ####");
        Log.i(TAG, "status: " + status + " " + (status == BluetoothGatt.GATT_SUCCESS ? "Success" : "Failure"));
        Log.i(TAG, "newState: " + newState);
        mDevice.setConnected(newState == BluetoothGatt.STATE_CONNECTED);
        if (status == BluetoothGatt.GATT_SUCCESS) {
            if (newState == BluetoothGatt.STATE_CONNECTED) {
                mDevice.getGatt().discoverServices();
            }
        }
        Log.i(TAG, "###################>");
    }

    @Override
    public void onServicesDiscovered(BluetoothGatt gatt, int status) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "#### onServicesDiscovered ####");
        Log.i(TAG, "###################>");
    }
}
