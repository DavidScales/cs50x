from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b (create a lookup table)"""

    d = [[]]
    LENGTH_A = len(a)
    LENGTH_B = len(b)

    # create the first row & column of lookup table - these values represent insertion or deletion of
    # an entire string, and are edge cases that would otherwise complicate our table population algorithm

    # add spaces
    a = ' ' + a
    b = ' ' + b

    # build first row
    for col in range(LENGTH_A):
        d[0].append(col)

    # build first column
    for row in range(1, LENGTH_B):
        d.append([row])

    # populate remain table contents
    for row in range(1, LENGTH_B):
        for col in range(1, LENGTH_A):
            # if the letters are the same
            if a[col] == b[row]:
                # the cost at this position is unchanged from the previous position
                previous = d[row - 1][col - 1]
                d[row].append(previous)
            else:
                # if the letters are different, the cost is the minimum of the exisiting cost + the cost of the edit
                # (currently the edit costs are unweighted - e.g., they all = 1)
                subsitution = d[row - 1][col - 1] + 1
                deletion = d[row][col - 1] + 1
                insertion = d[row - 1][col] + 1
                d[row].append(min(subsitution, deletion, insertion))

    # TODO - add operations

    for row in d:
        print(row)
    return d
