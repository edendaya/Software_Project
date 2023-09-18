from setuptools import setup, Extension

module = Extension('symnmfC',
                   sources=['symnmf.c','symnmfmodule.c'])

setup(name='symnmfC',
      version='1.0',
      description='Python wrapper for symnmf module C extension',
      ext_modules=[module])
