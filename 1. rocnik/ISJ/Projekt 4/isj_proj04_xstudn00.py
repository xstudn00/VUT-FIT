#!/usr/bin/env python3

import re

def can_be_a_set_member_or_frozenset(item):
    '''the function determines if the value can be added to the set'''
    new_item=str(item)
    tisk = re.sub( r'[^{}[\]]', '', new_item)
    if tisk is "":
        return item
    else:
        return frozenset(item)

def is_bit_set(num, bit):
    '''the function chcecks if the bit is set'''
    return num & (1 << bit) > 0

def all_subsets(lst):
    '''the function is making all subsets'''
    sets = []
    for i in range(1 << len(lst)):
        subset = [lst[bit] for bit in range(len(lst)) if is_bit_set(i, bit)]
        sets.append(subset)
    return sets


def all_subsets_excl_empty(*params, **exclude_empty):
    '''you can choose how to set looks like'''
    string = str(exclude_empty)
    new_string = (string[18::])
    if new_string == "False}":
        param=all_subsets(params)
        return param
    else:
        param=all_subsets(params)
        paraming=param.pop(0)
        return param

def test():
  '''testing all functions above'''  
  assert can_be_a_set_member_or_frozenset(1) == 1
  assert can_be_a_set_member_or_frozenset((1,2)) == (1,2)
  assert can_be_a_set_member_or_frozenset([1,2]) == frozenset([1,2])
  assert can_be_a_set_member_or_frozenset({'a':1,'b':2}) == frozenset(['a','b'])

  assert all_subsets(['a', 'b', 'c']) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

  assert all_subsets_excl_empty('a', 'b', 'c') == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
  assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = True) == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
  assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = False) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]



if __name__ == '__main__':
    test()
