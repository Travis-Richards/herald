#pragma once

class Api;
class QObject;
class QString;
class QStringList;

/// Creates a process API.
/// @param program The name or path to the program to run.
/// @param path The path to start the program in.
/// @param parent A pointer to the parent object.
/// @returns A new process API instance.
Api* make_process_api(const QString& program,
                      const QString& path,
                      const QStringList& args,
                      QObject* parent);
