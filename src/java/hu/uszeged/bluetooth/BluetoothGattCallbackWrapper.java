package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattCharacteristic;

final class BluetoothGattCallbackWrapper extends BluetoothGattCallback {
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
    }

    @Override
    public void onCharacteristicRead(
        BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
    }

    @Override
    public void onCharacteristicWrite(
        BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
    }

    @Override
    public void onDescriptorRead(
        BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
    }

    @Override
    public void onDescriptorWrite(
        BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
    }

    @Override
    public void onConnectionStateChange(
            BluetoothGatt gatt, int status, int newState) {
        mDevice.setConnected(newState == BluetoothGatt.STATE_CONNECTED);
        if (status == BluetoothGatt.GATT_SUCCESS) {
            if (newState == BluetoothGatt.STATE_CONNECTED) {
                mDevice.getGatt().discoverServices();
            }
        }
    }

    @Override
    public void onServicesDiscovered(
        BluetoothGatt gatt, int status) {
    }
}
