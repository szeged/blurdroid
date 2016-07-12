package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattDescriptor;
import android.util.Log;
import java.util.Set;
import java.util.HashSet;

final class BluetoothGattDescriptorWrapper {
    private static final String TAG = "BluetoothGattDescriptorWrapper";

    private BluetoothGattDescriptor mDescriptor;
    private BluetoothDeviceWrapper mDevice;
    private int mId;

    public BluetoothGattDescriptorWrapper(BluetoothGattDescriptor descriptor,
        BluetoothDeviceWrapper device, int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mDescriptor = descriptor;
        mDevice = device;
        mId = id;
        Log.i(TAG, "Descriptor: "+mDescriptor.getUuid().toString());
    }

    public static BluetoothGattDescriptorWrapper create(BluetoothGattDescriptor descriptor,
        BluetoothDeviceWrapper device, int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothGattDescriptorWrapper(descriptor, device, id);
    }

    public BluetoothGattDescriptor get() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "get");
        Log.i(TAG, "###################>");
        return mDescriptor;
    }

    public int getId() {
        return mId;
    }

    public String getUuid() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getUuid "+mDescriptor.getUuid().toString());
        Log.i(TAG, "###################>");
        return mDescriptor.getUuid().toString();
    }

    public int[] getValue() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getValue byte: "+mDescriptor.getValue());
        byte[] values = mDescriptor.getValue();
        if (values == null) {
            return null;
        }
        int[] intArray = new int[values.length];
        for(int i = 0, k = 0; i < values.length; i++) {
            intArray[i] = values[i] & (0xff);
        }
        Log.i(TAG, "getValue int: "+intArray);
        Log.i(TAG, "###################>");
        return intArray;
    }

    public int getValueSize() {
        int[] values = getValue();
        return (values == null) ? 0 : values.length;
    }

    public boolean setValue(int[] values) {
        // The values type is int,
        // but only containt u8 values from rust
        Log.i(TAG, "###################<");
        Log.i(TAG, "setValue int: "+values);
        byte[] byteArray = new byte[values.length];
        for(int i = 0, k = 0; i < values.length; i++) {
            byteArray[i] = (byte) values[i];
        }
        Log.i(TAG, "setValue byte: "+byteArray);
        Log.i(TAG, "###################>");
        return mDescriptor.setValue(byteArray);
    }

    public int[] readValue() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "readValue ");
        Log.i(TAG, "###################>");
        mDevice.getGatt().readDescriptor(this);
        return getValue();
    }

    public void writeValue(int[] values) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeValue "+values);
        Log.i(TAG, "###################>");
        setValue(values);
        mDevice.getGatt().writeDescriptor(this);
    }
}
