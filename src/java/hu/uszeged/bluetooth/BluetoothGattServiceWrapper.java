package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;

final class BluetoothGattServiceWrapper {
    private BluetoothGattService mService;
    private BluetoothDeviceWrapper mDevice;
    private HashMap<Integer, BluetoothGattCharacteristicWrapper> mCharacteristics;
    private int mId;

    public BluetoothGattServiceWrapper(BluetoothGattService service,
            BluetoothDeviceWrapper device) {
        mService = service;
        mDevice = device;
        mCharacteristics = new HashMap<Integer, BluetoothGattCharacteristicWrapper>();
        mId = service.hashCode();
    }

    public static BluetoothGattServiceWrapper create(BluetoothGattService service,
            BluetoothDeviceWrapper device) {
        return new BluetoothGattServiceWrapper(service, device);
    }

    public BluetoothGattService get() {
        return mService;
    }

    public int getId() {
        return mId;
    }

    public String getUuid() {
        return mService.getUuid().toString();
    }

    public int getInstanceId() {
        return mService.getInstanceId();
    }

    public int getType() {
        return mService.getType();
    }

    public boolean isPrimary() {
        return getType() == mService.SERVICE_TYPE_PRIMARY;
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        return mCharacteristics.get(id);
    }

    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        for (BluetoothGattCharacteristic characteristics : mService.getCharacteristics()) {
            addCharacteristic(characteristics);
        }
        return new HashSet<BluetoothGattCharacteristicWrapper>(mCharacteristics.values());
    }

    public void addCharacteristic(BluetoothGattCharacteristic characteristic) {
        if (!mCharacteristics.containsKey(characteristic.hashCode()))
            mCharacteristics.put(characteristic.hashCode(),
                BluetoothGattCharacteristicWrapper.create(characteristic, mDevice));
    }

    public int getCharacteristicsSize() {
        return mCharacteristics.values().size();
    }

    public Set<BluetoothGattServiceWrapper> getIncludedServices() {
        Set<BluetoothGattServiceWrapper> result
            = new HashSet<BluetoothGattServiceWrapper>();
        for (BluetoothGattService service : mService.getIncludedServices()) {
            result.add(BluetoothGattServiceWrapper.create(service, mDevice));
        }
        return result;
    }
}
