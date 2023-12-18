{ pkgs ? import <nixpkgs> { } }:
with pkgs;
mkShell {
  packages = [ libiconv darwin.apple_sdk.frameworks.SystemConfiguration ];
}
