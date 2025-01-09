{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  name = "Crypto";
  packages = with pkgs; [ 
    git
    clang
    gnumake
    gmp
  ];

  # ENVIRONMENT VARIABLES
  LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath [ pkgs.gmp ]}";
}
