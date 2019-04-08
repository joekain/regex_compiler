import os
import glob

DEFAULTS = ['build', "unit_test"]

def configure_defaults():
    global DOIT_CONFIG

    default_tasks = []
    if os.path.isfile('.local_dodo'):
        with open(".local_dodo") as f:
            for task in f:
                default_tasks.append(task.rstrip())
    else:
        default_tasks = DEFAULTS

    DOIT_CONFIG = {'default_tasks': default_tasks}


configure_defaults()
os. chdir("./build")


def task_cmake():
    """Generate Makefiles using cmake"""

    return {
        'targets' : ['Makefile'],
        'actions': ['cmake ..']
    }


def task_build():
    """Compile project"""

    return {
        'task_dep': ['cmake'],
        'actions': ['make']
    }


def post_build_task(actions):
    """Helper function to generate tasks that depend on 'build"""
    return {
        'task_dep' : ['build'],
        'actions' : actions,
    }


def task_unit_test():
    """Run unit tests"""
    return post_build_task(['ctest --output-on-failure'])


def task_clang_tidy():
    """Run clang tidy"""
    return post_build_task(['./run-clang-tidy.py -quiet;'])


def task_generate_pdf():
    """Generate PDFs from all DOT files"""
    dots = glob.glob("/tmp/*.dot")
    for dot_file in dots:
        pdf = dot_file.replace(".dot", ".pdf")
        yield {
            'task_dep' : ['build'],
            'name': pdf,
            'actions': ["dot -T pdf -O {}".format(dot_file)]
        }
