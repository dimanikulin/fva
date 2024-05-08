def test_decdeg2dms():
    # Test case 1: Positive decimal degrees
    dd = 45.6789
    expected_result = (45, 40, 44.04)
    assert decdeg2dms(dd) == expected_result

    # Test case 2: Negative decimal degrees
    dd = -12.3456
    expected_result = (-12, 20, 44.16)
    assert decdeg2dms(dd) == expected_result

    # Test case 3: Decimal degrees with zero minutes and seconds
    dd = 78.0
    expected_result = (78, 0, 0.0)
    assert decdeg2dms(dd) == expected_result

    # Test case 4: Decimal degrees with non-zero minutes and seconds
    dd = 123.4567
    expected_result = (123, 27, 24.12)
    assert decdeg2dms(dd) == expected_result

    # Test case 5: Decimal degrees equal to zero
    dd = 0.0
    expected_result = (0, 0, 0.0)
    assert decdeg2dms(dd) == expected_result

    print("All test cases passed!")

test_decdeg2dms()