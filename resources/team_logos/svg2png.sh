for svg in *.svg; do
    rsvg-convert -h 1024 "$svg" -o "${svg%.svg}.png"
done

