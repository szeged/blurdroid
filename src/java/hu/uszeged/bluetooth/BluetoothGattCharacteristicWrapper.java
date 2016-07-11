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
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mCharacteristic = characteristic;
        mDevice = device;
        Log.i(TAG, "Characteristic: " + mCharacteristic.getUuid().toString());
        Log.i(TAG, "Characteristic: " + mCharacteristic.getInstanceId());
        Log.i(TAG, "Characteristic: " + mCharacteristic.hashCode());
    }

    public static BluetoothGattCharacteristicWrapper create(BluetoothGattCharacteristic characteristic,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothGattCharacteristicWrapper(characteristic, device);
    }

    public BluetoothGattCharacteristic get() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeDescriptor");
        Log.i(TAG, "###################>");
        return mCharacteristic;
    }

    public String getUuid() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getUuid "+mCharacteristic.getUuid().toString());
        Log.i(TAG, "###################>");
        return mCharacteristic.getUuid().toString();
    }

    public int getInstanceId() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getInstanceId"+mCharacteristic.getInstanceId());
        Log.i(TAG, "###################>");
        return mCharacteristic.getInstanceId();
    }

    //TODO Set -> List
    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptors");
        Log.i(TAG, "###################>");
        for (BluetoothGattDescriptor descriptor : mCharacteristic.getDescriptors()) {
            mDevice.addDescriptor(descriptor);
        }
        return mDevice.getDescriptors();
    }

    /*public BluetoothGattDescriptorWrapper getDescriptor(String uuid) {
        Log.i(TAG, "getDescriptor");
        return mDevice.getDescriptor(uuid);
    }*/

    public BluetoothGattDescriptorWrapper getDescriptor(String uuid) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptor "+uuid);
        Log.i(TAG, "###################>");
        for (BluetoothGattDescriptorWrapper descriptor : mDevice.getDescriptors()) {
            if (descriptor.getUuid() == uuid) {
                return descriptor;           
            }
        }
        return null;
    }

    public BluetoothGattDescriptorWrapper getDescriptor(int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptor "+id);
        Log.i(TAG, "###################>");
        return mDevice.getDescriptor(id);
    }

    public byte[] getValue() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getValue "+mCharacteristic.getValue());
        Log.i(TAG, "###################>");
        return mCharacteristic.getValue();
    }

    public boolean setValue(byte[] value) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "setValue "+value);
        Log.i(TAG, "###################>");
        return mCharacteristic.setValue(value);
    }
}
