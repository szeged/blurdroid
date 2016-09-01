package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;

final class BluetoothGattCharacteristicWrapper {
    private BluetoothGattCharacteristic mCharacteristic;
    private BluetoothDeviceWrapper mDevice;
    private HashMap<Integer, BluetoothGattDescriptorWrapper> mDescriptors;
    private Set<String> mFlags;
    private int mId;

    public BluetoothGattCharacteristicWrapper(BluetoothGattCharacteristic characteristic,
            BluetoothDeviceWrapper device) {
        mCharacteristic = characteristic;
        mDevice = device;
        mDescriptors = new HashMap<Integer, BluetoothGattDescriptorWrapper>();
        mFlags = new HashSet<String>();
        mId = characteristic.hashCode();
        initFlags();
    }

    public static BluetoothGattCharacteristicWrapper create(BluetoothGattCharacteristic characteristic,
            BluetoothDeviceWrapper device) {
        return new BluetoothGattCharacteristicWrapper(characteristic, device);
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

    public Set<String> getFlags() {
        return mFlags;
    }

    public int getFlagsSize() {
        return mFlags.size();
    }

    public BluetoothGattDescriptorWrapper getDescriptor(int id) {
        return mDescriptors.get(id);
    }

    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        for (BluetoothGattDescriptor descriptor : mCharacteristic.getDescriptors()) {
            addDescriptor(descriptor);
        }
        return new HashSet<BluetoothGattDescriptorWrapper>(mDescriptors.values());
    }

    public void addDescriptor(BluetoothGattDescriptor descriptor) {
        if (!mDescriptors.containsKey(descriptor.hashCode()))
            mDescriptors.put(descriptor.hashCode(),
                BluetoothGattDescriptorWrapper.create(descriptor, mDevice));
    }

    public int getDescriptorsSize() {
        return mDescriptors.values().size();
    }

    public int[] getValue() {
        byte[] values = mCharacteristic.getValue();
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
        return mCharacteristic.setValue(byteArray);
    }

    public boolean readValue() {
        return mDevice.isConnected() && mDevice.getGatt().readCharacteristic(this);
    }

    public boolean writeValue(int[] values) {
        return mDevice.isConnected() && setValue(values) && mDevice.getGatt().writeCharacteristic(this);
    }

    private void initFlags() {
        int flag = mCharacteristic.getProperties();
        if ((flag & BluetoothGattCharacteristic.PROPERTY_BROADCAST) > 0) {
            mFlags.add("broadcast");
        }
        if ((flag & BluetoothGattCharacteristic.PROPERTY_READ) > 0) {
            mFlags.add("read");
        }
        if ((flag & BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE) > 0) {
            mFlags.add("write_without_response");
        }
        if ((flag & BluetoothGattCharacteristic.PROPERTY_WRITE) > 0) {
            mFlags.add("write");
        }
        if ((flag & BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0) {
            mFlags.add("notify");
        }
        if ((flag & BluetoothGattCharacteristic.PROPERTY_INDICATE) > 0) {
            mFlags.add("indicate");
        }
        if ((flag & BluetoothGattCharacteristic.PROPERTY_SIGNED_WRITE) > 0) {
            mFlags.add("authenticated_signed_writes");
        }
    }
}
