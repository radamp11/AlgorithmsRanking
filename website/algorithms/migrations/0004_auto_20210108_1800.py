# Generated by Django 3.1.4 on 2021-01-08 18:00

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('algorithms', '0003_algorithm_description'),
    ]

    operations = [
        migrations.AlterField(
            model_name='algorithm',
            name='description',
            field=models.CharField(default='brak opisu', max_length=30),
        ),
    ]