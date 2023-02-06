from setuptools import setup, Extension

setup(
        name= 'btree',
        version= '0.1',
     
        ext_modules= [
            Extension('btree', sources = ["btree.c"])
                ]
    )
