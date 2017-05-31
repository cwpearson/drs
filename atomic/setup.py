from distutils.core import setup, Extension

module1 = Extension('atomic',
                    sources = ['atomicmodule.c'])

setup (name = 'Atomic',
       version = '0.1',
       description = 'Atomic package for DRS',
       ext_modules = [module1])
