package main

import (
	"math/bits"
	"math"
	"fmt"
)

func SlashNotation(mask uint32) int {
	return 32 - bits.TrailingZeros32(mask)
}

func ReadQuad() uint32 {
	var a, b, c, d uint32
	fmt.Scanf("%d.%d.%d.%d", &a, &b, &c, &d)
	return a << 24 + b << 16 + c << 8 + d
}

func SlashToMask(slash uint32) uint32 {
	return 0xffffffff & ^(2 << (32 - slash - 1) - 1)
}

func ClassToMask(class rune) uint32 {
	switch class {
		case 'A': return 8
		case 'B': return 16
		case 'C': return 24
	}
	return 0
}

func ToClass(ip uint32) rune {
	switch ip >> 24 & 0xc0 {
		case 0x00: return 'A'
		case 0x80: return 'B'
		case 0xc0: return 'C'
	}
	return ' '
}

func Print(ip uint32) string {
	return fmt.Sprintf("%d.%d.%d.%d",
		(ip & 0xff000000) >> 24,
		(ip & 0x00ff0000) >> 16,
		(ip & 0x0000ff00) >> 8,
		(ip & 0x000000ff));
}

type Network struct {
	Start        uint32
	End          uint32
	MagicNumber  uint32
	Hosts        uint32
}

func NextPowerOfTwo(v uint32) uint32 {
	v -= 1
	v |= v >> 1
	v |= v >> 2
	v |= v >> 4
	v |= v >> 8
	v |= v >> 16
	return v + 1
}

func MagicNumber(mask uint32) uint32 {
	p := uint32((bits.LeadingZeros32(^mask) / 8) * 8)
	return 256 - mask << p >> p
}

func FindSubnet(ip uint32, hosts uint32) Network {
	h := NextPowerOfTwo(hosts + 2)
	c := uint32(math.Log2(float64(h)))

	m := MagicNumber(SlashToMask(32 - c))

	return Network{ip, ip + m - 1, m, h}
}

func main() {
	ip := ReadQuad()
	var hosts uint32
	fmt.Scanf("%d", &hosts)

	s := FindSubnet(ip, hosts)
	fmt.Printf("%s, %s", Print(s.Start), Print(s.End))
}

