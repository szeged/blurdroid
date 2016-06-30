package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.util.Log;
import java.util.Set;
import java.util.HashSet;
import java.util.UUID;

final class BluetoothGattCharacteristicWrapper {
    private static final String TAG = "BluetoothGattCharacteristicWrapper";

    private BluetoothGattCharacteristic mCharacteristic;
    private BluetoothDeviceWrapper mDevice;

    public BluetoothGattCharacteristicWrapper(BluetoothGattCharacteristic characteristic,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "ctor");
        mCharacteristic = characteristic;
        mDevice = device;
    }

    public static BluetoothGattCharacteristicWrapper create(BluetoothGattCharacteristic characteristic,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "create");
        return new BluetoothGattCharacteristicWrapper(characteristic, device);
    }

    public BluetoothGattCharacteristic get() {
        Log.i(TAG, "writeDescriptor");
        return mCharacteristic;
    }

    public String getUuid() {
        Log.i(TAG, "getUuid");
        return mCharacteristic.getUuid().toString();
    }

    public int getInstanceId() {
        Log.i(TAG, "getInstanceId");
        return mCharacteristic.getInstanceId();
    }

    //TODO Set -> List
    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        Log.i(TAG, "getDescriptors");
        for (BluetoothGattDescriptor descriptor : mCharacteristic.getDescriptors()) {
            mDevice.addDescriptor(descriptor);
        }
        return mDevice.getDescriptors();
    }

    public BluetoothGattDescriptorWrapper getDescriptor(String uuid) {
        Log.i(TAG, "getDescriptor");
        return mDevice.getDescriptor(uuid);
    }

    public byte[] getValue() {
        Log.i(TAG, "getValue");
        return mCharacteristic.getValue();
    }

    public boolean setValue(byte[] value) {
        Log.i(TAG, "setValue");
        return mCharacteristic.setValue(value);
    }
}
