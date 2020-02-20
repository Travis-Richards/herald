#pragma once

class Api;
class QObject;
class QString;
class QStringList;

namespace herald {

template <typename T>
class ScopedPtr;

class Model;

/// Used for constructing process API instances.
class ProcessApiFactory {
public:
  /// Creates a new instance of a process API factory.
  static ScopedPtr<ProcessApiFactory> make();
  /// Just a stub.
  virtual ~ProcessApiFactory() {}
  /// Creates a new process API instance.
  /// @param parent A pointer to the parent object for the process.
  /// @returns A new process API instance.
  virtual Api* make_process_api(QObject* parent) = 0;
  /// Sets the additional arguments to the process.
  /// @param args Additional arguments to assign the process.
  virtual void set_args(const QStringList& args) = 0;
  /// Assigns the model that the process API will be modifying.
  /// @param model A pointer to the game model to modify.
  virtual void set_model(Model* model) = 0;
  /// Sets the path to the program to start.
  /// @param program The path to the program to start.
  virtual void set_program(const QString& program) = 0;
  /// Sets the working directory for the process to start in.
  /// @param pwd The working directory to start the process in.
  virtual void set_working_directory(const QString& pwd) = 0;
};

} // namespace herald
