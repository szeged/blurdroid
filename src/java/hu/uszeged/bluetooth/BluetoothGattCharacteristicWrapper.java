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
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mCharacteristic = characteristic;
        mDevice = device;
        mId = id;
        Log.i(TAG, "Characteristic: " + mCharacteristic.getUuid().toString());
    }

    public static BluetoothGattCharacteristicWrapper create(BluetoothGattCharacteristic characteristic,
        BluetoothDeviceWrapper device, int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothGattCharacteristicWrapper(characteristic, device, id);
    }

    public BluetoothGattCharacteristic get() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "get");
        Log.i(TAG, "###################>");
        return mCharacteristic;
    }

    public int getId() {
        return mId;
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

    public int getDescriptorsSize() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptorsSize");
        Log.i(TAG, "###################>");
        return mDevice.getDescriptorsSize();
    }

    public int[] getValue() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getValue byte: "+Arrays.toString(mCharacteristic.getValue()));
        byte[] values = mCharacteristic.getValue();
        if (values == null) {
            return null;
        }
        int[] intArray = new int[values.length];
        for(int i = 0, k = 0; i < values.length; i++) {
            intArray[i] = values[i] & (0xff);
        }
        Log.i(TAG, "getValue int: "+Arrays.toString(intArray));
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
        Log.i(TAG, "setValue int: "+Arrays.toString(values));
        byte[] byteArray = new byte[values.length];
        for(int i = 0, k = 0; i < values.length; i++) {
            byteArray[i] = (byte) values[i];
        }
        Log.i(TAG, "setValue byte: "+Arrays.toString(byteArray));
        Log.i(TAG, "###################>");
        return mCharacteristic.setValue(byteArray);
    }

    public int[] readValue() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "readValue ");
        Log.i(TAG, "###################>");
        mDevice.getGatt().readCharacteristic(this);
        return getValue();
    }

    public void writeValue(int[] values) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeValue "+Arrays.toString(values));
        Log.i(TAG, "###################>");
        setValue(values);
        mDevice.getGatt().writeCharacteristic(this);
    }
}
