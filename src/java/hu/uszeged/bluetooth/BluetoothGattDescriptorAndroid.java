package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattDescriptor;
import android.util.Log;
import java.util.Set;
import java.util.HashSet;

final class BluetoothGattDescriptorWrapper {
    private static final String TAG = "BluetoothGattDescriptorWrapper";

    private BluetoothGattDescriptor mDescriptor;
    private BluetoothDeviceWrapper mDevice;

    public BluetoothGattDescriptorWrapper(BluetoothGattDescriptor descriptor,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "ctor");
        mDescriptor = descriptor;
        mDevice = device;
    }

    public static BluetoothGattDescriptorWrapper create(BluetoothGattDescriptor descriptor,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "create");
        return new BluetoothGattDescriptorWrapper(descriptor, device);
    }

    public BluetoothGattDescriptor get() {
        Log.i(TAG, "get");
        return mDescriptor;
    }

    public String getUuid() {
        Log.i(TAG, "getUuid");
        return mDescriptor.getUuid().toString();
    }

    public byte[] getValue() {
        Log.i(TAG, "getValue");
        return mDescriptor.getValue();
    }

    public boolean setValue(byte[] value) {
        Log.i(TAG, "setValue");
        return mDescriptor.setValue(value);
    }
}
