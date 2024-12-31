import unreal

EditorAssetLibrary = unreal.EditorAssetLibrary

# suffixes
COMPRESSION_MAPPING = {
    "_N": unreal.TextureCompressionSettings.TC_NORMALMAP,   # Normal map
    "_D": unreal.TextureCompressionSettings.TC_DEFAULT,     # Albedo/Diffuse
    "_E": unreal.TextureCompressionSettings.TC_DEFAULT,     # Emissive
    "_M": unreal.TextureCompressionSettings.TC_DEFAULT,     # Metallic
    "_R": unreal.TextureCompressionSettings.TC_GRAYSCALE,   # Roughness
}


def validate_compression_settings(directory: str, apply_fix: bool = True):
    asset_path_list = EditorAssetLibrary.list_assets(directory, recursive=True)
    for asset_path in asset_path_list:
        texture = EditorAssetLibrary.load_asset(asset_path)

        # Must be Texture2D
        if not isinstance(texture, unreal.Texture2D):
            continue

        # Check if the texture has a suffix that matches the compression mapping
        name = str(texture.get_fname())
        name_match = False
        correct_compression = None
        for suffix in COMPRESSION_MAPPING.keys():
            if name.endswith(suffix):
                name_match = True
                correct_compression = COMPRESSION_MAPPING[suffix]
                break
        if not name_match:
            continue

        current_compression = texture.get_editor_property("compression_settings")
        if current_compression != correct_compression:
            print(f"WRONG COMPRESSION SETTINGS ON: {asset_path}")
            if apply_fix:
                print(f"{asset_path} compression was set to {str(correct_compression)}")
                texture.set_editor_property(name="compression_settings", value=correct_compression)


if __name__ == "__main__":
    validate_compression_settings("/Game/StarterContent/Textures", apply_fix=True)
