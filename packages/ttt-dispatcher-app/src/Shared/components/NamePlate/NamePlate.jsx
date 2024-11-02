import React from 'react';
import './NamePlate.scss'
import Badge from '@mui/material/Badge';
import Chip from '@mui/material/Chip';

const limitString = (str, maxLength) => {
  if (str.length <= maxLength) {
    return str;
  } else {
    return str.slice(0, maxLength);
  }
};

export const NamePlate = ({
  name,
  size,
  consistCount = 0,
  sx = {},
  disabled = false,
  onClick = () => { }
}) => {

  return (
    <Badge
      badgeContent={consistCount}
      color="info"
      invisible={consistCount < 2}>
      <Chip
        className="throttle-nameplate"
        label={limitString(name, 20)}
        size={size}
        disabled={disabled}
        onClick={onClick}
        variant="outlined">
      </Chip>
    </Badge>)
}

export default NamePlate