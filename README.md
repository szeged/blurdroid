# blurdroid
### Build
```
$ #set ANDROID_NDK and ANDROID_SDK env
$ rustup target add arm-linux-androideabi
$ cargo build --target=arm-linux-androideabi
```
### Usage
Min sdk api: 18 (Android 4.3)

Copy the `blurdroid.jar` from the `out` directory to your `<android-project>/libs` directory.
