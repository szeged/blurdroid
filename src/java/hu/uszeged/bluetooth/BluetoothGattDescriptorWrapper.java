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

    public byte[] getValue() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getValue "+mDescriptor.getValue());
        Log.i(TAG, "###################>");
        return mDescriptor.getValue();
    }

    public boolean setValue(byte[] value) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "setValue "+value);
        Log.i(TAG, "###################>");
        return mDescriptor.setValue(value);
    }
}
