package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattDescriptor;
import android.util.Log;
import java.util.Arrays;
import java.util.Set;
import java.util.HashSet;

final class BluetoothGattDescriptorWrapper {
    private static final String TAG = "BluetoothGattDescriptorWrapper";

    private BluetoothGattDescriptor mDescriptor;
    private BluetoothDeviceWrapper mDevice;
    private int mId;

    public BluetoothGattDescriptorWrapper(BluetoothGattDescriptor descriptor,
        BluetoothDeviceWrapper device, int id) {
        mDescriptor = descriptor;
        mDevice = device;
        mId = id;
    }

    public static BluetoothGattDescriptorWrapper create(BluetoothGattDescriptor descriptor,
        BluetoothDeviceWrapper device, int id) {
        return new BluetoothGattDescriptorWrapper(descriptor, device, id);
    }

    public BluetoothGattDescriptor get() {
        return mDescriptor;
    }

    public int getId() {
        return mId;
    }

    public String getUuid() {
        return mDescriptor.getUuid().toString();
    }

    public int[] getValue() {
        byte[] values = mDescriptor.getValue();
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
        return mDescriptor.setValue(byteArray);
    }

    public int[] readValue() {
        mDevice.getGatt().readDescriptor(this);
        return getValue();
    }

    public void writeValue(int[] values) {
        setValue(values);
        mDevice.getGatt().writeDescriptor(this);
    }
}
