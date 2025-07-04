CWD=$(PWD)
OS_NAME=$(uname -s)
BUILD_PATH="$CWD/product"

rm -rf $BUILD_PATH
mkdir $BUILD_PATH

if [[ "$OS_NAME" == "Darwin" ]]; then
    echo "Running product build on MacOS"

    # Build release flavour
    cmake -B$BUILD_PATH -DCMAKE_MACOSX_BUNDLE=ON -DCMAKE_BUILD_TYPE=Release -DPACKAGE_PRODUCT=ON
    cmake --build $BUILD_PATH -- -j$(sysctl -n hw.ncpu)

    # Deploy QT plugins
    QT_PLUG_SRC="/opt/homebrew/share/qt/plugins"
    QT_PLUG_DST="$BUILD_PATH/bin/CollisionCraft.app/Contents/PlugIns"
    echo "Deploying QT plugins -> $QT_PLUG_DST"

    mkdir -p "$QT_PLUG_DST"
    mkdir -p "$QT_PLUG_DST/iconengines"
    mkdir -p "$QT_PLUG_DST/imageformats"
    mkdir -p "$QT_PLUG_DST/platforminputcontexts"
    mkdir -p "$QT_PLUG_DST/platform"
    mkdir -p "$QT_PLUG_DST/styles"

    cp -r "$QT_PLUG_SRC/iconengines"* "$QT_PLUG_DST/iconengines"
    cp -r "$QT_PLUG_SRC/imageformats"* "$QT_PLUG_DST/imageformats"
    cp -r "$QT_PLUG_SRC/platforminputcontexts"* "$QT_PLUG_DST/platforminputcontexts"
    cp -r "$QT_PLUG_SRC/platform"* "$QT_PLUG_DST/platform"
    cp -r "$QT_PLUG_SRC/styles"* "$QT_PLUG_DST/styles"

    find $BUILD_PATH/bin/CollisionCraft.app/Contents/PlugIns -name "*.dylib"  | while read dylib; do
        install_name_tool -add_rpath /opt/homebrew/opt/qt@6/lib "$dylib"
    done

    # Deploy OpenUSD plugins
    USD_PLUG_SRC="$CWD/lib/openusd/macos/lib/usd"
    USD_PLUG_DST="$BUILD_PATH/bin/CollisionCraft.app/Contents/Frameworks"
    echo "Deploying USD plugins -> $USD_PLUG_DST"
    mkdir -p "$USD_PLUG_DST"
    cp -r "$USD_PLUG_SRC"* "$USD_PLUG_DST"

    # Package dependencies & code sign.
    echo "Deploying package dependencies..."
    /opt/homebrew/opt/qt@6/bin/macdeployqt $BUILD_PATH/bin/CollisionCraft.app -dmg -verbose=1

    echo "Signing package..."
    codesign --deep --force --verify --verbose --sign "-" "$BUILD_PATH/bin/CollisionCraft.app"


elif [[ "$OS_NAME" == "Linux" ]]; then
    echo "Running product build on Linux"
else
    echo "Unsupported OS: $OS_NAME - aborting"
fi
