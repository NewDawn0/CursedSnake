{
  description = "ScuffedSnake - A cursed snake game";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, ... }@inputs:
    inputs.flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        pname = "ssnake";
        buildCmd = ''
          rm -rf bins
          mkdir -p bins
          g++ -Wall -Wextra -g -o bins/${pname} src/*.cpp
        '';
        buildInputs = with pkgs; [ fltk libcxx ];
        nativeBuildInputs = with pkgs; [ gcc ];
      in with pkgs; {
        devShell = mkShell {
          inherit buildInputs nativeBuildInputs;
          shellHook = ''
            alias build="${buildCmd}"
            alias run="build ./bins/${pname}"
            function loop() {
              while true; do
                echo "Building..."
                eval "$1" ''${@:2}
                read -p "Press [Enter] key to rerun..." && clear
              done
            }
          '';
        };
        packages.default = stdenv.mkDerivation {
          inherit pname;
          version = "0.0.1";
          src = ./.;
          inherit buildInputs nativeBuildInputs;
          packages = [ clang-tools ];
          buildPhase = "${buildCmd}";
          installPhase = ''
            mkdir -p $out/bin
            cp bins/ssnake $out/bin/
          '';
        };
      });
}
