use std::env;
use std::path::Path;
use std::process;
use std::process::{Command, Stdio};

fn main() {
    // build.rs is not platform-specific, so we have to check the target here.
    let target = env::var("TARGET").unwrap();
    if target.contains("android") {
        android_main()
    }
}

fn android_main() {
    let ndk_path = env::var("NDK_HOME").ok().expect("Please set the NDK_HOME environment variable");
    let ndk_path = Path::new(&ndk_path);

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

    println!("cargo:rustc-link-lib=static=blurdroid");
    println!("cargo:rustc-link-search=native={}", out_dir);
}