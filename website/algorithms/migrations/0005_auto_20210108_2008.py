# Generated by Django 3.1.4 on 2021-01-08 20:08

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('algorithms', '0004_auto_20210108_1800'),
    ]

    operations = [
        migrations.AddField(
            model_name='algorithm',
            name='results10',
            field=models.FileField(default='no-file', upload_to=''),
        ),
        migrations.AlterField(
            model_name='algorithm',
            name='alg_file',
            field=models.FileField(upload_to='zip_files/'),
        ),
    ]
