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
        mDevice = device;
    }

    public static BluetoothGattCallbackWrapper create(
            BluetoothDeviceWrapper device) {
        return new BluetoothGattCallbackWrapper(device);
    }

    @Override
    public void onCharacteristicChanged(
            BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
        Log.i(TAG, "onCharacteristicChanged");
    }

    @Override
    public void onCharacteristicRead(
            BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
        Log.i(TAG, "onCharacteristicRead");
    }

    @Override
    public void onCharacteristicWrite(
            BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
        Log.i(TAG, "onCharacteristicWrite");
    }

    @Override
    public void onDescriptorRead(
            BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
        Log.i(TAG, "onDescriptorRead");
    }

    @Override
    public void onDescriptorWrite(
            BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
        Log.i(TAG, "onDescriptorWrite");
    }

    @Override
    public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
        Log.i(TAG, "#### onConnectionStateChange ####");
        mDevice.getGatt().discoverServices();
    }

    @Override
    public void onServicesDiscovered(BluetoothGatt gatt, int status) {
        Log.i(TAG, "#### onServicesDiscovered ####");
    }
}
