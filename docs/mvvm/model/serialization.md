# Serialization

We assume that persistent data is stored in XML files

+ Serialization is used to undo/redo
+ For deep item copying and cloning
+ To save persistent context on disk

## Examples of serialization

Normally, `SessionItem`

### `PropertyItem` with data

```C++
PropertyItem item;
item.setData(42, DataRole::kData);
item.setData("width", DataRole::kDisplay);
```

```XML
<Item model="Property">
  <ItemData>
    <Variant role="0" type="string">{8f923bfc-94b3-456e-b222-0c81f19b8f5f}</Variant>
    <Variant role="1" type="int">42</Variant>
    <Variant role="2" type="string">width</Variant>
  </ItemData>
  <TaggedItems defaultTag=""/>
</Item>
```

### Parent with single child

```C++
SessionItem parent;
parent.setDisplayName("parent_name");
parent.registerTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

auto child = parent.insertItem(std::make_unique<PropertyItem>(), TagIndex::append());
child->setDisplayName("child_name");
```

```XML
<?xml version="1.0" encoding="UTF-8"?>
<Item type="SessionItem">
  <ItemData>
    <Variant role="0" type="string">{ca0fc80b-1246-4a69-8896-c6df46e3aa99}</Variant>
    <Variant role="2" type="string">parent_name</Variant>
  </ItemData>
  <TaggedItems defaultTag="defaultTag">
    <ItemContainer>
      <TagInfo max="-1" min="0" name="defaultTag"/>
      <Item type="Property">
        <ItemData>
          <Variant role="0" type="string">{9459511d-1096-4fa1-a3e8-eb1d7386694d}</Variant>
          <Variant role="2" type="string">child_name</Variant>
        </ItemData>
        <TaggedItems defaultTag=""/>
      </Item>
    </ItemContainer>
  </TaggedItems>
</Item>
```

## Related files

+ See `treedataitemconverter.test.cpp` with examples of the SessionItem serialization.


## Draft

### `variant_t`

```json
{
  "type": "int",
  "value": 42
},
{
  "type": "std::vector<double>",
  "value": [
    42.1,
    42.2,
    42.3
  ]
},
{
  "type": "std::string",
  "value": "abc"
},
{
  "type": "ModelView::ExternalProperty",
  "value": {
    "color": "#ff00ff00",
    "identifier": "1-2-3",
    "text": "abc"
  }  
}
```

### SessionItemData

```json
[
    {
        "role": 1,
        "variant": {
            "type": "int",
            "value": 42
        }
    },
    {
        "role": 2,
        "variant": {
            "type": "double",
            "value": 1.23
        }
    },
    {
        "role": 3,
        "variant": {
            "type": "std::string",
            "value": "abc"
        }
    }
]
```

## PropertyItem

```json
{
    "itemData": [
        {
            "role": 0,
            "variant": {
                "type": "std::string",
                "value": "{862c6e9b-2d1e-427d-a80d-22c592c50b36}"
            }
        },
        {
            "role": 2,
            "variant": {
                "type": "std::string",
                "value": "Property"
            }
        }
    ],
    "itemTags": {
        "containers": [
        ],
        "defaultTag": ""
    },
    "model": "Property"
}

```


```xml
<ItemData>
  <Variant role="0" type="int">42</Variant>
  <Variant role="0" type="std::vector<double>">42.1, 42.2, 42.3</Variant>
</ItemData>

<ItemData>
  <Variant type="int">42</Variant>
  <Variant type="std::vector<double>">42.1, 42.2, 42.3</Variant>
</ItemData>

```

```xml
<TaggedItems defaultTag="defaultTag">
  <ItemContainer>
    <TagInfo min="-1" max="0" name="defaultTag">models, models, models</TagInfo>
    <Item model="Property"> 
      <ItemData/>
      <TaggedItems/>
    </Item>
  </ItemContainer>
  <ItemContainer>
  </ItemContainer>
</TaggedItems>
```

```
  std::unique_ptr<TreeData> ToTreeData(const SessionItemData& item_data) const;
  std::unique_ptr<SessionItemData> ToSessionItemData(const TreeData& tree_data) const;

  std::unique_ptr<TreeData> ToTreeData(const TaggedItems& tagged_items) const;
  std::unique_ptr<TaggedItems> ToTaggedItems(const TreeData& tree_data) const;

  std::unique_ptr<TreeData> ToTreeData(const SessionItemData& item_data) const;
  std::unique_ptr<SessionItemData> FromTreeData(const TreeData& tree_data) const;

  std::unique_ptr<TreeData> FromSessionItemData(const SessionItemData& item_data) const;
  std::unique_ptr<SessionItemData> ToSessionItemData(const TreeData& tree_data) const;

  std::unique_ptr<TreeData> FromTaggedItems(const SessionItemData& item_data) const;
  std::unique_ptr<SessionItemData> ToSessionItemData(const TreeData& tree_data) const;

```