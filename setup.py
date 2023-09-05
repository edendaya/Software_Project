from setuptools import setup, Extension

setup(
    name='MyModule',
    ext_modules=[
        Extension('mymodule', sources=['symnmfmodule.c', 'symnmf.c'])
    ],
)
