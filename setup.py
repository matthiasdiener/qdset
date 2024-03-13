from setuptools import setup, Extension

def main():
    setup(name="qdset",
          version="2024.0",
          description="qdset",
          author="Matthias Diener",
          author_email="mdiener@illinois.edu",
          ext_modules=[Extension("qdset", ["setobject.c"],
                                 extra_compile_args=["-g"],
                                 depends=["setobject.h"])
                       ],
          )

if __name__ == "__main__":
    main()
