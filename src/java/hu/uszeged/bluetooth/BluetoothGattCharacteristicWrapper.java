package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.util.Log;
import java.util.Arrays;
import java.util.Set;
import java.util.HashSet;
import java.util.UUID;

final class BluetoothGattCharacteristicWrapper {
    private static final String TAG = "BluetoothGattCharacteristicWrapper";

    private BluetoothGattCharacteristic mCharacteristic;
    private BluetoothDeviceWrapper mDevice;
    private int mId;

    public BluetoothGattCharacteristicWrapper(BluetoothGattCharacteristic characteristic,
        BluetoothDeviceWrapper device, int id) {
        mCharacteristic = characteristic;
        mDevice = device;
        mId = id;
    }

    public static BluetoothGattCharacteristicWrapper create(BluetoothGattCharacteristic characteristic,
        BluetoothDeviceWrapper device, int id) {
        return new BluetoothGattCharacteristicWrapper(characteristic, device, id);
    }

    public BluetoothGattCharacteristic get() {
        return mCharacteristic;
    }

    public int getId() {
        return mId;
    }

    public String getUuid() {
        return mCharacteristic.getUuid().toString();
    }

    public int getInstanceId() {
        return mCharacteristic.getInstanceId();
    }

    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        for (BluetoothGattDescriptor descriptor : mCharacteristic.getDescriptors()) {
            mDevice.addDescriptor(descriptor);
        }
        return mDevice.getDescriptors();
    }

    public BluetoothGattDescriptorWrapper getDescriptor(String uuid) {
        for (BluetoothGattDescriptorWrapper descriptor : mDevice.getDescriptors()) {
            if (descriptor.getUuid() == uuid) {
                return descriptor;           
            }
        }
        return null;
    }

    public BluetoothGattDescriptorWrapper getDescriptor(int id) {
        return mDevice.getDescriptor(id);
    }

    public int getDescriptorsSize() {
        return mDevice.getDescriptorsSize();
    }

    public int[] getValue() {
        byte[] values = mCharacteristic.getValue();
        if (values == null) {
            return null;
        }
        int[] intArray = new int[values.length];
        for(int i = 0, k = 0; i < values.length; i++) {
            intArray[i] = values[i] & (0xff);
        }
        return intArray;
    }

    public int getValueSize() {
        int[] values = getValue();
        return (values == null) ? 0 : values.length;
    }

    public boolean setValue(int[] values) {
        // The values type is int,
        // but only containt u8 values from rust
        byte[] byteArray = new byte[values.length];
        for(int i = 0, k = 0; i < values.length; i++) {
            byteArray[i] = (byte) values[i];
        }
        return mCharacteristic.setValue(byteArray);
    }

    public int[] readValue() {
        mDevice.getGatt().readCharacteristic(this);
        return getValue();
    }

    public void writeValue(int[] values) {
        setValue(values);
        mDevice.getGatt().writeCharacteristic(this);
    }
}
