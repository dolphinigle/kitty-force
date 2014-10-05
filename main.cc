/*
 * Licensed under GNU GPL v3.0.
 * (p.s., license roughly means derivatives of this software must remain open-source).
 *
 * Welcome to the world of the magical dolphinigle qtcreator Codeforces framework!
 *
 * Setup:
 *   1) Create a bin folder for your executables.
 *   2) In your project, build, build steps, add build step, custom process step.
 *      Here, command is "cmake", argument is ".. -DCMAKE_BUILD_TYPE=Debug"
 *
 * Features:
 *   a) debug problem A with test case stored in file a1.in
 *      run main in your project, and as arguments, give "a a1.in".
 *
 *      note: the file a1.in is located in the same location with the source.
 *   b) Run all test cases for problem A
 *      run main in your project, and as argument, give "a".
 *      it will run against all test cases: everything whose regex matches a*.in
 *      if the file has the corresponding a*.out, it will match the output as well, and tell you
 *      if they match. The outputs are streamed to stdout.
 */

#include <glob.h>
#include <unistd.h>
#include <sys/wait.h>

// Include other files you wish to support here. Sadly C++11 support for reflection is lacking,
// thus if you add other files here, you have to modify the "if executable_name == "a"" thingies
// below and add your thing.
#include "a.cc"
#include "b.cc"
#include "c.cc"
#include "d.cc"
#include "e.cc"

const string TIMEOUT = "5";  // TODO(irvan): "fix?" :3

int main(int args, char** argv) {
  if (args == 3) {
    // executable_name input_name
    // This will run here so that its debuggable.
    string executable_name = argv[1];
    string input_filename = argv[2];
    string input_filename_path = "../" + input_filename;
    // TODO(irvan): this is a hack to support both executing from inside bin/ and outside:
    if (input_filename[0] == '.') {
      input_filename_path = input_filename;
    }
    printf("Executing %s on input %s\n", executable_name.c_str(), input_filename_path.c_str());

    // Redirect stdin to file.
    freopen(input_filename_path.c_str(), "r", stdin);

    if (executable_name == "a") {
      main_a();
    } else if (executable_name == "b") {
      main_b();
    } else if (executable_name == "c") {
      main_c();
    } else if (executable_name == "d") {
      main_d();
    } else if (executable_name == "e") {
      main_e();
    } else {
      printf("Unrecognized executable name %s\n", executable_name.c_str());
      return 1;
    }
    printf("\n");
    return 0;
  } else if (args == 2) {
    string executable_name = argv[1];
    string pattern = "../" + executable_name + "*.in";

    glob_t glob_result;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);

    vector<string> paths;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
      paths.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);

    printf("Found %d input files for %s.\n", (int)paths.size(), executable_name.c_str());
    fflush(stdout);

    for (auto path: paths) {
      pid_t pid = fork();
      switch (pid) {
        case -1: { // Bad thing happened.
          std::cerr << "fork() has failed. This, dolphinigle cannot fix.\n";
          exit(1);
          break;
        }
        case 0: {
            execl("/usr/bin/timeout", "/usr/bin/timeout", TIMEOUT.c_str(), "./main", executable_name.c_str(), path.c_str(), NULL);
            std::cerr << "Something bad happened when executing " <<
                         executable_name << " on " << path << endl;
            exit(1);
            break;
        }
        default: {
            int status;
            while (waitpid(pid, &status, 0) == -1);
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
              std::cerr << "Failed.\n";
            } else {
              fflush(stdout);
            }
            break;
        }
      }
    }
    printf("\n");
    return 0;
  } else {
    printf("Usage: main <exec_name> [input_filename]");
    return 1;
  }
}









