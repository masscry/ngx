#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse as arg
import struct as st
import os.path as ph

def parse():
    parser = arg.ArgumentParser( description='Convert OBJ model to NMD.')
    parser.add_argument('filename', type=str, help='Input OBJ model filename')

    args = parser.parse_args()
    return args

def entry():
    args = parse()

    v = []
    vn = []
    vt = []
    f = []

    buffer = []

    with open(args.filename, 'r') as input:
        for line in input:
            data = line.strip()
            if data[0] == '#':
                continue

            parts = data.split()

            if parts[0] == 'v':
                v.append([float(x) for x in parts[1:]])
            elif parts[0] == 'vn':
                vn.append([float(x) for x in parts[1:]])
            elif parts[0] == 'vt':
                vt.append([float(x) for x in parts[1:]])
            elif parts[0] == 'f':

                if (len(vt) == 0):
                    vt.append( [0.0, 0.0] )
                if (len(vn) == 0):
                    vn.append( [0.0, 0.0, 0.0] )

                for face in parts[1:]:
                    vnd = [ (int(x)-1) if len(x) > 0 else 0 for x in face.split('/')]

                    buffer.append( v[vnd[0]] + vt[vnd[1]] + vn[vnd[2]] )

            else:
                print line

    output = ph.splitext(args.filename)[0] + '.nmd'

    with open(output, 'w') as ofile:
        header = st.pack('=HH', 2, len(buffer))
        ofile.write(header)

        for vrt in buffer:
            bvrt = st.pack('=ffffffff', *vrt)
            ofile.write(bvrt)


if __name__ == '__main__':
    entry()

