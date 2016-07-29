use std::env;
use std::path::Path;
use std::process;
use std::process::{Command, Stdio};
use std::fs;

fn main() {
    // build.rs is not platform-specific, so we have to check the target here.
    let target = env::var("TARGET").unwrap();
    if target.contains("android") {
        android_main()
    }
}

fn android_main() {
    let ndk_path = env::var("ANDROID_NDK").ok().expect("Please set the ANDROID_NDK environment variable");
    let ndk_path = Path::new(&ndk_path);
    let sdk_path = env::var("ANDROID_SDK").ok().expect("Please set the ANDROID_SDK environment variable");
    let sdk_path = Path::new(&sdk_path);

    if Command::new(ndk_path.join("ndk-build"))
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
        .arg("src/obj/local/armeabi/libblurdroid.a")
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
        .arg(sdk_path.join("platforms/android-18/android.jar"))
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
