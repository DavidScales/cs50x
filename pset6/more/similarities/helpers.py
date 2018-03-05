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

    # add spaces to simplify indexing
    a = ' ' + a
    b = ' ' + b
    LENGTH_A = len(a)
    LENGTH_B = len(b)

    # create the first row & column of lookup table - these values represent insertion or deletion of
    # an entire string, and are edge cases that would otherwise complicate our table population algorithm

    # build the corner
    d[0].append((0, None))

    # build first row
    for col in range(1, LENGTH_B):
        d[0].append((col, Operation.INSERTED))

    # build first column
    for row in range(1, LENGTH_A):
        d.append([(row, Operation.DELETED)])

    # populate remain table contents
    for row in range(1, LENGTH_A):
        for col in range(1, LENGTH_B):
            # if the letters are the same
            if a[row] == b[col]:
                # the cost at this position is unchanged from the previous position
                previous = d[row - 1][col - 1][0]
                d[row].append((previous, Operation.SUBSTITUTED)) # using SUBSTITUTED because application.py logic expects that,
                                                                  # even though nothing is actually substituted
            else:
                # if the letters are different, the cost is the minimum of the exisiting cost + the cost of the edit
                # (currently the edit costs are unweighted - e.g., they all = 1)
                subsitution = (d[row - 1][col - 1][0] + 1, Operation.SUBSTITUTED)
                deletion = (d[row][col - 1][0] + 1, Operation.INSERTED)
                insertion = (d[row - 1][col][0] + 1, Operation.DELETED)
                d[row].append(min(subsitution, deletion, insertion, key = lambda item: item[0]))

    return d
