from distutils.core import setup, Extension

def main():
    setup(name="qdset",
          version="1.0.0",
          description="qdset",
          author="<your name>",
          author_email="your_email@gmail.com",
          ext_modules=[Extension("qdset", ["setobject.c"])])

if __name__ == "__main__":
    main()
