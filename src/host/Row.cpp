/********************************************************
 *                                                       *
 *   Copyright (C) Microsoft. All rights reserved.       *
 *                                                       *
 ********************************************************/

#include "precomp.h"

// Routine Description:
// - swaps two ROWs
// Arguments:
// - a - the first ROW to swap
// - b - the second ROW to swap
// Return Value:
// - <none>
void swap(ROW& a, ROW& b) noexcept
{
    a.swap(b);
}

// Routine Description:
// - constructor
// Arguments:
// - rowId - the row index in the text buffer
// - rowWidth - the width of the row, cell elements
// - fillAttribute - the default text attribute
// Return Value:
// - constructed object
ROW::ROW(_In_ const SHORT rowId, _In_ const short rowWidth, _In_ const TextAttribute fillAttribute) :
    sRowId{ rowId },
    CharRow{ rowWidth },
    AttrRow{ rowWidth, fillAttribute }
{
}

// Routine Description:
// - copy constructor
// Arguments:
// - a - the object to copy
// Return Value:
// - the copied object
ROW::ROW(const ROW& a) :
    CharRow{ a.CharRow },
    AttrRow{ a.AttrRow },
    sRowId{ a.sRowId }
{
}

// Routine Description:
// - assignment operator overload
// Arguments:
// - a - the object to copy to this one
// Return Value:
// - a reference to this object
ROW& ROW::operator=(const ROW& a)
{
    ROW temp{ a };
    this->swap(temp);
    return *this;
}

// Routine Description:
// - move constructor
// Arguments:
// - a - the object to move
// Return Value:
// - the constructed object
ROW::ROW(ROW&& a) noexcept :
    CharRow{ std::move(a.CharRow) },
    AttrRow{ std::move(a.AttrRow) },
    sRowId{ std::move(a.sRowId) }
{
}

// Routine Description:
// - swaps fields with another ROW
// Arguments:
// - other - the object to swap with
// Return Value:
// - <none>
void ROW::swap(ROW& other) noexcept
{
    using std::swap;
    swap(CharRow, other.CharRow);
    swap(AttrRow, other.AttrRow);
    swap(sRowId, other.sRowId);
}

const CHAR_ROW& ROW::GetCharRow() const
{
    return CharRow;
}

CHAR_ROW& ROW::GetCharRow()
{
    return const_cast<CHAR_ROW&>(static_cast<const ROW* const>(this)->GetCharRow());
}

const ATTR_ROW& ROW::GetAttrRow() const
{
    return AttrRow;
}

ATTR_ROW& ROW::GetAttrRow()
{
    return const_cast<ATTR_ROW&>(static_cast<const ROW* const>(this)->GetAttrRow());
}

SHORT ROW::GetId() const noexcept
{
    return sRowId;
}

void ROW::SetId(_In_ const SHORT id)
{
    sRowId = id;
}

// Routine Description:
// - Sets all properties of the ROW to default values
// Arguments:
// - sRowWidth - The width of the row.
// - Attr - The default attribute (color) to fill
// Return Value:
// - <none>
bool ROW::Reset(_In_ short const sRowWidth, _In_ const TextAttribute Attr)
{
    CharRow.Reset(sRowWidth);
    return AttrRow.Reset(sRowWidth, Attr);
}

// Routine Description:
// - resizes ROW to new width
// Arguments:
// - width - the new width, in cells
// Return Value:
// - S_OK if successful, otherwise relevant error
HRESULT ROW::Resize(_In_ size_t const width)
{
    size_t oldWidth = CharRow.size();
    RETURN_IF_FAILED(CharRow.Resize(width));
    RETURN_IF_FAILED(AttrRow.Resize(static_cast<short>(oldWidth), static_cast<short>(width)));
    return S_OK;
}

// Routine Description:
// - clears char data in column in row
// Arguments:
// - column - 0-indexed column index
// Return Value:
// - <none>
void ROW::ClearColumn(_In_ const size_t column)
{
    THROW_HR_IF(E_INVALIDARG, column >= CharRow.size());
    CharRow.ClearGlyph(column);
    CharRow.GetAttribute(column).SetSingle();
}
