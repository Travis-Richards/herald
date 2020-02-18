#pragma once

class QString;

namespace herald {

class Model;

/// Loads the data from the legacy model.
/// @param model The model to put the data into.
/// @param game_path The path to the game directory.
/// @returns True on success, false on failure.
bool load_legacy_model(Model* model, const QString& game_path);

} // namespace herald
