package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattDescriptor;

final class BluetoothGattDescriptorWrapper {
    private BluetoothGattDescriptor mDescriptor;
    private BluetoothDeviceWrapper mDevice;
    private int mId;

    public BluetoothGattDescriptorWrapper(BluetoothGattDescriptor descriptor,
            BluetoothDeviceWrapper device) {
        mDescriptor = descriptor;
        mDevice = device;
        mId = descriptor.hashCode();
    }

    public static BluetoothGattDescriptorWrapper create(BluetoothGattDescriptor descriptor,
            BluetoothDeviceWrapper device) {
        return new BluetoothGattDescriptorWrapper(descriptor, device);
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
        if (values == null)
            return null;

        int[] intArray = new int[values.length];
        for(int i = 0; i < values.length; i++) {
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
        for(int i = 0; i < values.length; i++) {
            byteArray[i] = (byte) values[i];
        }
        return mDescriptor.setValue(byteArray);
    }

    public boolean readValue() {
        return mDevice.isConnected() && mDevice.getGatt().readDescriptor(this);
    }

    public boolean writeValue(int[] values) {
        return mDevice.isConnected() && setValue(values) && mDevice.getGatt().writeDescriptor(this);
    }
}
