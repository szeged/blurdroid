package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.util.Log;
import java.util.Set;
import java.util.HashSet;
import java.util.UUID;

final class BluetoothGattServiceWrapper {
    private static final String TAG = "BluetoothGattServiceWrapper";

    private BluetoothGattService mService;
    private BluetoothDeviceWrapper mDevice;

    public BluetoothGattServiceWrapper(BluetoothGattService service,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "ctor");
        mService = service;
        mDevice = device;
    }

    public static BluetoothGattServiceWrapper create(BluetoothGattService service,
        BluetoothDeviceWrapper device) {
        Log.i(TAG, "create");
        return new BluetoothGattServiceWrapper(service, device);
    }

    public BluetoothGattService get() {
        Log.i(TAG, "get");
        return mService;
    }

    public String getUuid() {
        Log.i(TAG, "getUuid");
        return mService.getUuid().toString();
    }

    public int getInstanceId() {
        Log.i(TAG, "getInstanceId");
        return mService.getInstanceId();
    }

    public int getType() {
        Log.i(TAG, "getType");
        return mService.getType();
    }

    public boolean isPrimary() {
        Log.i(TAG, "isPrimary");
        return getType() == mService.SERVICE_TYPE_PRIMARY;
    }

    //TODO Set -> List
    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        Log.i(TAG, "getCharacteristics");
        for (BluetoothGattCharacteristic characteristics : mService.getCharacteristics()) {
            mDevice.addCharacteristic(characteristics);
        }
        return mDevice.getCharacteristics();
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(String uuid) {
        Log.i(TAG, "getCharacteristic");
        for (BluetoothGattCharacteristicWrapper characteristic : mDevice.getCharacteristics()) {
            if (characteristic.getUuid() == uuid) {
                return characteristic;           
            }
        }
        return null;
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        Log.i(TAG, "getCharacteristic");
        return mDevice.getCharacteristic(id);
    }

    //TODO Set -> List
    public Set<BluetoothGattServiceWrapper> getIncludedServices() {
        Log.i(TAG, "getIncludedServices");
        Set<BluetoothGattServiceWrapper> result
            = new HashSet<BluetoothGattServiceWrapper>();
        for (BluetoothGattService service : mService.getIncludedServices()) {
            result.add(BluetoothGattServiceWrapper.create(service, mDevice));
        }
        return result;
    }
}
