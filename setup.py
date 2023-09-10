from setuptools import Extension, setup

module = Extension("symnmf", sources=['symnmf.c'])
setup(name='symnmf',
      version='1.0',
      description='Python wrapper for symnmf module C extension',
      ext_modules=[module])
