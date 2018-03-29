use std::env;
use std::path::{Path, PathBuf};
use std::process;
use std::process::{Command, Stdio};
use std::fs;

fn main() {
    let target = env::var("TARGET").unwrap();
    if target.contains("android") {
        android_main()
    } else {
        println!("Android target supported only");
    }
}

fn android_main() {
    let ndk_path = env::var("ANDROID_NDK").ok().expect("Please set the ANDROID_NDK environment variable");
    let ndk_path = Path::new(&ndk_path);
    let sdk_path = env::var("ANDROID_SDK").ok().expect("Please set the ANDROID_SDK environment variable");
    let sdk_path = Path::new(&sdk_path);

    let target = env::var("TARGET").unwrap();
    let abi = if target.contains("armv7") {
        "armeabi-v7a"
    } else if target.contains("aarch64") {
        "arm64-v8a"
    } else if target.contains("arm") {
        "armeabi"
    } else if target.contains("x86") || target.contains("i686") {
        "x86"
    } else {
        panic!("Invalid target architecture {}", target);
    };

    if Command::new(ndk_path.join("ndk-build"))
        .arg(format!("APP_ABI={}", abi))
        .arg("-C")
        .arg("src/jni/")
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status().unwrap().code().unwrap() != 0
    {
        println!("Error while executing ndk-build");
        process::exit(1)
    }

    let out_dir = env::var("OUT_DIR").ok().expect("Cargo should have set the OUT_DIR environment variable");

    if Command::new("cp")
        .arg(format!("src/obj/local/{}/libblurdroid.a", abi))
        .arg(&format!("{}", out_dir))
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status().unwrap().code().unwrap() != 0
    {
        println!("Error while executing cp");
        process::exit(1)
    }

    if Command::new("mkdir")
        .arg("-p")
        .arg("target/java/classes/")
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status().unwrap().code().unwrap() != 0
    {
        println!("Error while executing cp");
        process::exit(1)
    }

    let java_files = fs::read_dir("src/java/hu/uszeged/bluetooth").unwrap().map(|p| p.unwrap().path()).collect::<Vec<_>>();
    let java_files = java_files.iter().map(|p| p.as_os_str().to_str().unwrap()).collect::<Vec<_>>();

    if Command::new("javac")
        .arg("-d")
        .arg("target/java/classes/")
        .arg("-cp")
        .arg(find_android_platform(sdk_path))
        .args(java_files.as_slice())
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status().unwrap().code().unwrap() != 0
    {
        println!("Error while executing javac");
        process::exit(1)
    }

    if Command::new("jar")
        .arg("cvf")
        .arg(&format!("{}/blurdroid.jar", out_dir))
        .arg("-C")
        .arg("target/java/classes/")
        .arg("hu")
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status().unwrap().code().unwrap() != 0
    {
        println!("Error while executing jar");
        process::exit(1)
    }

    println!("cargo:rustc-link-lib=static=blurdroid");
    println!("cargo:rustc-link-search=native={}", out_dir);
}

fn find_android_platform(sdk_path: &Path) -> PathBuf {
    // Sorted android platforms by priority
    let mut platforms = Vec::new();
    // Allow configuration using env variable
    if let Ok(platform) =  env::var("ANDROID_SDK_PLATFORM") {
        platforms.push(platform);
    }
    // Fallback to other platforms
    for v in (18..27).rev() {
        platforms.push(v.to_string());
    }

    for platfom in platforms {
        let path = sdk_path.join("platforms").join(format!("android-{}", platfom)).join("android.jar");
        if path.exists() {
            return path;
        }
    }

    panic!("Couldn't find a correct android SDK platform. Please set ANDROID_SDK_PLATFORM env variable");
}
